#!/usr/bin/bash

# enter several new lines to make it obvious where this started
echo
echo
echo
echo

# Generate first four digits of kit serial number.
fourdig=$(python3 kitnum.py)
# Ask user for last two digits of kit serial number.
echo ""
echo Today\'s serial number starts with... $fourdig
echo "     -----------"
echo "     | $fourdig-?? |"
echo "     -----------"
echo Please enter two digits to replace ??
echo "  01 for the first kit of the day:"
echo "    ??: 01"
echo "  Or Enter to use a custom serial number."
echo
read -p '    ??: ' twodig
echo ""

# make input_is_ok 0 if input is not OK
input_is_ok=1
# handle case user entered NO digits
if [ -z "$twodig" ]
then
    echo Enter a custom serial number \(just hit Enter to cancel\)
    read -p '    ????-??: ' dashed_number
    # handle case user cancels
    if [ -z "$dashed_number" ]
    then
        echo ""
        echo OK, cancelled :\) No files created.
        echo ""
        return -1
    # validate user's custom serial number
    else
    #
    # TODO: handle case serial number has too many dashes.
    #
    # TODO: troubleshoot this case: not detecting dash
    # handle case serial number is missing a dash
        if [[ "${dashed_number}" == *"\-"* ]]
        then
            echo Hmmm... serial number is missing a dash.
            input_is_ok=0
        # split serial number at the dash
        else
            # set delimiter to '-'
            IFS='-'
            # read custom serial number into array SN
            read -a SN <<< "$dashed_number"
            # restore delimiter to space
            IFS=' '
            # handle case first four characters contain non-digits
            if [[ "${SN[0]}" =~ [^0-9]+ ]]
            then
                echo Hmmm... entered symbols other than 0,1,2,3,4,5,6,7,8,9.
                input_is_ok=0
            # handle case next two characters contain non-digits
            elif [[ "${SN[1]}" =~ [^0-9]+ ]]
            then
                echo Hmmm... entered symbols other than 0,1,2,3,4,5,6,7,8,9.
                input_is_ok=0
            # handle case first part of serial number is not exactly four digits
            elif [ "${#SN[0]}" -ne 4 ]
            then
                echo Hmmm... first part of serial number must be four digits.
                input_is_ok=0
            # handle case second part of serial number is not exactly two digits
            elif [ "${#SN[1]}" -ne 2 ]
            then
                echo Hmmm... second part of serial number must be two digits.
                input_is_ok=0
            fi
            # exit if any of the above failed
            if [ $input_is_ok -ne 1 ]
            then
                echo Exiting :\( No files created.
                echo ""
                return -1
            fi
        fi
    fi
# handle case user entered ANY non-digits
elif [[ "$twodig" =~ [^0-9]+ ]]
then
    echo Hmmm... entered symbols other than 0,1,2,3,4,5,6,7,8,9.
    input_is_ok=0
# handle case user entered too few digits
elif [ "${#twodig}" -lt 2 ]
then
    echo Hmmm... entered too few digits.
    input_is_ok=0
# handle case user entered too many digits
elif [ "${#twodig}" -gt 2 ]
then
    echo Hmmm... entered too many digits.
    input_is_ok=0
# handle case user entered two digits as expected
else
    dashed_number=$fourdig-$twodig
fi
# exit if any of the above failed
if [ $input_is_ok -ne 1 ]
then
    echo Exiting :\( No files created.
    echo ""
    return -1
fi

echo OK, serial number is:
echo "     -----------"
echo "     | $dashed_number |"
echo "     -----------"
echo ""
echo "Creating folder and XML file for kit..."

# Create a folder for this kit if it does not exist.
if [ -e kits/$dashed_number/ ]
then
    echo "    ...did NOT create folder in kits/"
    echo "       folder kits/$dashed_number/ already exists"
else
    mkdir -p kits/$dashed_number/
    echo "    ...created folder kits/$dashed_number/"
fi

# Put an ftcfg.xml in this folder for programming EEPROM.
echo $dashed_number | python3 writexml.py
mv newftcfg.xml kits/$dashed_number/ftcfg.xml

echo ""
echo "Run \"make progkitnum\" to program the kit with this serial number."
