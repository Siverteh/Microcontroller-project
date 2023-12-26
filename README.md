# IKT104G-22V-exam

To use this project, you have to 

1. Install the HTS221 library to Mbed Studio:
    You can use the existing HTS221.lib file under the libs folder and add the library automatically through
    MBeds library view by clicking on the exclamation mark.

    If it does not exist:
    Under Mbed Studios Libraries view add a new library with url:
    https://os.mbed.com/teams/ST/code/HTS221/

2. Add wifi module:
    You can use the existing COMPONENT_ism43362.lib file in the libs folder. Go to the Libraries view in
    Mbed and add the library from there.
    
    If it does not exist:
    Create file libs/COMPONENT_ism43362.lib
    File should contain "https://github.com/ARMmbed/wifi-ism43362/"
    You may have to click fix problems under the Libraries view in Mbed to fix the module.
	
3. Use an mbed_app.json file as described in "SAMPLE mbed_app.json.txt".