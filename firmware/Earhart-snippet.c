#include <avr/io.h>
#include <avr/interrupt.h>
#include "modules/adc/adc.h"
#include "modules/FT1248/FT1248.h"
#include "modules/pwm/pwm.h"
void setup(void);
void loop(void);
int
main (void) // The program counter points here on power-up/reset.
{
    setup();    // Call setup() only once.
    for ( ; ; )  // Call loop() in an infinite loop.
    {
        loop();
    }
    return 0;
}
typedef enum // labview_message_t (data type for a local variable)
{   //  Message 0x0000, 0x0001, and 0x0002 are undefined.
    LABV_MSG__SEND_FRAME = 3,
    LABV_MSG__SET_NPIX, // 4, etc.
    LABV_MSG__SET_INTEGRATION_TIME,
    // Force the compiler to make sizeof(labview_message_t) uint16_t.
    END_OF_LIST_OF_POSSIBLE_MESSAGES = 0xFFFF

} labview_message_t;
volatile struct // gf_labview_tasks
{
    // request_frame == the host PC wants a frame.
    uint8_t request_adc_frame : 1;

    // send_frame == OK to send readings to the host PC.
    uint8_t send_adc_frame : 1;

    // echo == _T if there is data to echo back to the host PC.
    uint8_t data_to_echo : 1;
} gf_labview_tasks;
// LabVIEW has control over npix; the initial value is based on the TSL201/1401.
uint16_t g_npix = NUM_PIXELS;
// The minimum allowed integration time is the time req'd to clock N+1 pixels.
uint16_t g_clk_top = NUM_PIXELS + 1;
// Count the CLK tics.
volatile uint16_t g_clk_cnt;
// Updates to the frame size and integration time are permitted only at the
// beginning of the frame. Store the new values in the _next variables.
uint16_t g_npix_next = NUM_PIXELS;
uint16_t g_clk_top_next = NUM_PIXELS + 1;

//  Variable for echoing back undefined commands.
uint16_t g_data_to_echo;

//  ft1248 read/write ret Bool for success/fail; for future err-chk impl.
uint8_t g_ft1248_status;

// TO DO: REWRITE A COMPLETE SET OF setup() and loop() functions
// for each compile case.
void
setup (void)
{
    CFG_TSL_SI_PIN_AS_OUTPUT;
    //  Configure the PWM module.
    pwm_cfg();
    //  Turn on the PWM output.
    pwm_on();
    //  Enable an interrupt to trigger on the PWM rising edge.
    pwm_enable_rising_edge_interrupt();
    //  Enable an interrupt to trigger on the PWM falling edge.
    pwm_enable_falling_edge_interrupt();
    //  Configure the USI module for FT1248 serial communication.
    ft1248_cfg();
    // Configure the ADC for single-shot, single-ended, external AREF readings.
    adc_cfg();
}

void
loop (void)
{
    // Check for incoming data from the host PC.
    if (ft1248_available())
    {
        // Store the incoming data in variable lv_msg.
        labview_message_t lv_msg;

        // Pass a pointer to lv_msg so that the ft1248_read function
        // can write directly to the lv_msg variable.
        g_ft1248_status = ft1248_read(&lv_msg);
        // TO DO: use ft1248_status for error checking.

        // Determine the correct response to the LabVIEW message.
        switch(lv_msg)
        {
            case LABV_MSG__SEND_FRAME:
                // This flag is checked during the CLK fedg ISR.
                gf_labview_tasks.request_adc_frame = _T;
                break;
            case LABV_MSG__SET_NPIX:
                // The _next value is loaded during the CLK fedg ISR.
                g_ft1248_status = ft1248_read(&g_npix_next);
                break;
            case LABV_MSG__SET_INTEGRATION_TIME:
                // The _next value is loaded during the CLK fedg ISR.
                g_ft1248_status = ft1248_read(&g_clk_top_next);
                break;
            default:
                /*  Unknown message received.
                    Echo it back to LabVIEW.
                    In the 4/8/2012 version of Graph_Taos_Pixel_Frames.vi,
                    the generation of lv_msg is automated, so
                    this default case should never happen.
                    If it does, there will be two unread bytes
                    (for each unknown command received)
                    sitting in the serial buffer when the VI is
                    stopped with the big, red, square button.*/
                gf_labview_tasks.data_to_echo = _T;
                g_data_to_echo = lv_msg;
                break;
        }
    }
    // If an unrecognized command was received, echo it back to the host PC.
    if (gf_labview_tasks.data_to_echo)
    {
        // Send the data.
        if(ft1248_write(g_data_to_echo))
        {
            // The write succeeded. Clear the flag.
            gf_labview_tasks.data_to_echo = _F;
        }
        // The FT1248 master failed to write the data.
        else
        {
            // Do nothing. The next iteration of loop() tries to write again.
        }
    }
}

// This interrupt triggers on the rising edge of CLK.
ISR(TIM0_COMPA_vect)
{
    // Increment the CLK tic counter.
    g_clk_cnt++;
    // Check if the frame is finished yet.
    if (g_clk_cnt >= g_clk_top)
    {
        // Reset the counter.
        g_clk_cnt = 0;
        // The frame is done: clear the flag.
        gf_labview_tasks.send_adc_frame = _F;
    }
    else if (g_clk_cnt <= g_npix)
    {
        // Create storage for the ADC result.
        uint16_t adc_reading;

        // Take a reading.
        adc_reading = adc_read();

        // Check if this frame is OK to transmit.
        if (gf_labview_tasks.send_adc_frame)
        {
            // Send the reading.
            g_ft1248_status = ft1248_write(adc_reading);
        }
    }
}
// This interrupt triggers on the falling edge of CLK.
ISR(TIM0_COMPB_vect)
{
    if (0 != g_clk_cnt)
    {
        // Pull SI low.
        TSL_SI_PIN_LOW;
#if _BOARD == _LINDBERGH
        // Pull HOLD low.
        TSL_HOLD_PIN_LOW;
#endif // TSL1401
    }
    else
    {
        // Pull SI high.
        TSL_SI_PIN_HIGH;
#if _BOARD == _LINDBERGH
        // Pull HOLD high.
        TSL_HOLD_PIN_HIGH;
#endif // _BOARD == _LINDBERGH
        // Check if a frame was requested.
        if (gf_labview_tasks.request_adc_frame)
        {
            // Prepare to send the next full frame.
            gf_labview_tasks.request_adc_frame = _F;
            gf_labview_tasks.send_adc_frame = _T;

            // Make sure g_clk_top is valid.
            g_clk_top_next = (g_clk_top_next < (g_npix_next + 1)) ? g_npix_next + 1 : g_clk_top_next;
            // Update g_npix and g_clk_top with their new values.
            g_npix = g_npix_next;
            g_clk_top = g_clk_top_next;

            // Send the values of npix and clk_top used in the next full frame.
            g_ft1248_status = ft1248_write(g_npix);
            g_ft1248_status = ft1248_write(g_clk_top);
        }
    }
}

