#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''System tests of dev-kit-mike/firmware.

USAGE:

    Run tests with:
    $ pytest --testdox system-tests.py

    Mike's VIM shortcut: ;uf
'''

# I use unittest to automate these system tests
import unittest

# Use Sean's simple interface
from chromaspeclib.simple import ChromaSpecSimpleInterface

# Open communication. Communication closes when this script finishes.
kit = ChromaSpecSimpleInterface(
    serial_number='091103', # dev-kit I'm using for my system tests
    timeout=0.1 # timeout if there is no response in 100ms
    )

# -------------
# | Constants |
# -------------

# status
OK = 0
ERROR = 1

# led_setting
OFF = 0
GREEN = 1
RED = 2

# LIS-770i config
BINNING_OFF = 0
BINNING_ON  = 1
GAIN_1X  = 0x01
GAIN_2X5 = 0x25
GAIN_4X  = 0x04
GAIN_5X  = 0x05
ALL_ROWS_ACTIVE   = 0x1F
ONLY_ROW_1_ACTIVE = 0x01
ONLY_ROW_2_ACTIVE = 0x02
ONLY_ROW_3_ACTIVE = 0x04
ONLY_ROW_4_ACTIVE = 0x08
ONLY_ROW_5_ACTIVE = 0x10

# command_id
GetBridgeLED = 1
SetBridgeLED = 2
GetSensorLED = 3
SetSensorLED = 4
GetSensorConfig = 7
SetSensorConfig = 8
GetExposure = 9
SetExposure = 10
CaptureFrame = 11

# --------------------
# | API System Tests |
# --------------------

class TestGetBridgeLED(unittest.TestCase):

    def test_GetBridgeLED_1_replies_status_ERROR(self):
        # =====[ Operate ]=====
        reply = kit.getBridgeLED(1)
        self.assertEqual(GetBridgeLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(ERROR, reply.status)

    def test_GetBridgeLED_0_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.getBridgeLED(0)
        self.assertEqual(GetBridgeLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_GetBridgeLED_0_replies_led_setting_0_if_OFF(self):
        # =====[ Setup ]=====
        kit.setBridgeLED(0,OFF)
        # =====[ Operate ]=====
        reply = kit.getBridgeLED(0)
        self.assertEqual(GetBridgeLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OFF, reply.led_setting)

    def test_GetBridgeLED_0_replies_led_setting_1_if_GREEN(self):
        # =====[ Setup ]=====
        kit.setBridgeLED(0,GREEN)
        # =====[ Operate ]=====
        reply = kit.getBridgeLED(0)
        self.assertEqual(GetBridgeLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(GREEN, reply.led_setting)

    def test_GetBridgeLED_0_replies_led_setting_1_if_RED(self):
        # =====[ Setup ]=====
        kit.setBridgeLED(0,RED)
        # =====[ Operate ]=====
        reply = kit.getBridgeLED(0)
        self.assertEqual(GetBridgeLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(RED, reply.led_setting)

class TestSetBridgeLED(unittest.TestCase):

    def test_SetBridgeLED_1_X_replies_status_ERROR(self):
        # =====[ Operate ]=====
        reply = kit.setBridgeLED(1,1)
        self.assertEqual(SetBridgeLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(ERROR, reply.status)

    def test_SetBridgeLED_0_OFF_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setBridgeLED(0,OFF)
        self.assertEqual(SetBridgeLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_SetBridgeLED_0_GREEN_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setBridgeLED(0,GREEN)
        self.assertEqual(SetBridgeLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_SetBridgeLED_0_RED_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setBridgeLED(0,RED)
        self.assertEqual(SetBridgeLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

class TestGetSensorLED(unittest.TestCase):

    def test_GetSensorLED_2_replies_status_ERROR(self):
        # =====[ Operate ]=====
        reply = kit.getSensorLED(2)
        self.assertEqual(GetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(ERROR, reply.status)

    def test_GetSensorLED_1_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.getSensorLED(1)
        self.assertEqual(GetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_GetSensorLED_0_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.getSensorLED(0)
        self.assertEqual(GetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_GetSensorLED_0_replies_led_setting_0_if_OFF(self):
        # =====[ Setup ]=====
        kit.setSensorLED(0,OFF)
        # =====[ Operate ]=====
        reply = kit.getSensorLED(0)
        self.assertEqual(GetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OFF, reply.led_setting)

    def test_GetSensorLED_0_replies_led_setting_0_if_GREEN(self):
        # =====[ Setup ]=====
        kit.setSensorLED(0,GREEN)
        # =====[ Operate ]=====
        reply = kit.getSensorLED(0)
        self.assertEqual(GetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(GREEN, reply.led_setting)

    def test_GetSensorLED_0_replies_led_setting_0_if_RED(self):
        # =====[ Setup ]=====
        kit.setSensorLED(0,RED)
        # =====[ Operate ]=====
        reply = kit.getSensorLED(0)
        self.assertEqual(GetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(RED, reply.led_setting)

    def test_GetSensorLED_1_replies_led_setting_0_if_OFF(self):
        # =====[ Setup ]=====
        kit.setSensorLED(1,OFF)
        # =====[ Operate ]=====
        reply = kit.getSensorLED(1)
        self.assertEqual(GetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OFF, reply.led_setting)

    def test_GetSensorLED_1_replies_led_setting_0_if_GREEN(self):
        # =====[ Setup ]=====
        kit.setSensorLED(1,GREEN)
        # =====[ Operate ]=====
        reply = kit.getSensorLED(1)
        self.assertEqual(GetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(GREEN, reply.led_setting)

    def test_GetSensorLED_1_replies_led_setting_0_if_RED(self):
        # =====[ Setup ]=====
        kit.setSensorLED(1,RED)
        # =====[ Operate ]=====
        reply = kit.getSensorLED(1)
        self.assertEqual(GetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(RED, reply.led_setting)

class TestSetSensorLED(unittest.TestCase):

    def test_SetSensorLED_2_X_replies_status_ERROR(self):
        # =====[ Operate ]=====
        reply = kit.setSensorLED(2,OFF)
        self.assertEqual(SetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(ERROR, reply.status)

    def test_SetSensorLED_0_OFF_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setSensorLED(0,OFF)
        self.assertEqual(SetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_SetSensorLED_0_GREEN_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setSensorLED(0,GREEN)
        self.assertEqual(SetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_SetSensorLED_0_RED_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setSensorLED(0,RED)
        self.assertEqual(SetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_SetSensorLED_1_OFF_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setSensorLED(1,OFF)
        self.assertEqual(SetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_SetSensorLED_1_GREEN_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setSensorLED(1,GREEN)
        self.assertEqual(SetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_SetSensorLED_1_RED_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setSensorLED(1,RED)
        self.assertEqual(SetSensorLED, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

class TestSetSensorConfig(unittest.TestCase):

    def test_SetSensorConfig_BINNING_OFF_GAIN_5X_ONLY_ROW_3_ACTIVE_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.setSensorConfig(BINNING_OFF, GAIN_5X, ONLY_ROW_3_ACTIVE)
        self.assertEqual(SetSensorConfig, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_SetSensorConfig_BINNING_2_GAIN_5X_ONLY_ROW_3_ACTIVE_replies_status_ERROR(self):
        # =====[ Operate ]=====
        reply = kit.setSensorConfig(2, GAIN_5X, ONLY_ROW_3_ACTIVE)
        self.assertEqual(SetSensorConfig, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(ERROR, reply.status)

class TestGetSensorConfig(unittest.TestCase):

    def test_GetSensorConfig_replies_with_parameters_matching_most_recently_set_config(self):
        # =====[ Setup ]=====
        kit.setSensorConfig(BINNING_OFF, GAIN_5X, ONLY_ROW_3_ACTIVE)
        # =====[ Operate ]=====
        reply = kit.getSensorConfig()
        self.assertEqual(GetSensorConfig, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(BINNING_OFF, reply.binning)
        self.assertEqual(GAIN_5X, reply.gain)
        self.assertEqual(ONLY_ROW_3_ACTIVE, reply.row_bitmap)

    def test_GetSensorConfig_replies_BINNING_ON_GAIN_1X_ALL_ROWS_ACTIVE_if_using_default_config(self):
        # =====[ Setup ]=====
        kit.setSensorConfig(BINNING_ON, GAIN_1X, ALL_ROWS_ACTIVE)
        # =====[ Operate ]=====
        reply = kit.getSensorConfig()
        self.assertEqual(GetSensorConfig, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(BINNING_ON, reply.binning)
        self.assertEqual(GAIN_1X, reply.gain)
        self.assertEqual(ALL_ROWS_ACTIVE, reply.row_bitmap)

class TestGetExposure(unittest.TestCase):

    def test_GetExposure_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.getExposure()
        self.assertEqual(GetExposure, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_GetExposure_replies_with_50_exposure_cycles_if_using_default_exposure(self):
        # =====[ Setup ]=====
        cycles=50
        kit.setExposure(cycles)
        # =====[ Operate ]=====
        reply = kit.getExposure()
        self.assertEqual(GetExposure, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(cycles, reply.cycles)

    def test_GetExposure_replies_with_exposure_cycles_matching_most_recently_set_exposure(self):
        # =====[ Setup ]=====
        exposure = 10e-3 # seconds
        cycles = round(exposure/20e-6) # one exposure cycle is 20µs
        kit.setExposure(cycles)
        # =====[ Operate ]=====
        reply = kit.getExposure()
        self.assertEqual(GetExposure, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(cycles, reply.cycles)

class TestSetExposure(unittest.TestCase):

    def test_SetExposure_replies_status_OK(self):
        exposure = 10e-3 # seconds
        cycles = round(exposure/20e-6) # one exposure cycle is 20µs
        # =====[ Operate ]=====
        reply = kit.setExposure(cycles)
        self.assertEqual(SetExposure, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

class TestCaptureFrame(unittest.TestCase):

    def test_CaptureFrame_replies_status_OK(self):
        # =====[ Operate ]=====
        reply = kit.captureFrame()
        self.assertEqual(CaptureFrame, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(OK, reply.status)

    def test_CaptureFrame_replies_num_pixels_is_392(self):
        # =====[ Setup ]=====
        kit.setSensorConfig(BINNING_ON, GAIN_1X, ALL_ROWS_ACTIVE)
        kit.setExposure(50) # 1ms
        # =====[ Operate ]=====
        reply = kit.captureFrame()
        self.assertEqual(CaptureFrame, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(392, reply.num_pixels)

    def test_CaptureFrame_replies_with_392_16bit_pixel_values(self):
        # =====[ Setup ]=====
        kit.setSensorConfig(BINNING_ON, GAIN_1X, ALL_ROWS_ACTIVE)
        kit.setExposure(50) # 1ms
        # =====[ Operate ]=====
        reply = kit.captureFrame()
        self.assertEqual(CaptureFrame, reply.command_id)
        # =====[ Test ]=====
        self.assertEqual(392, len(reply.pixels))
