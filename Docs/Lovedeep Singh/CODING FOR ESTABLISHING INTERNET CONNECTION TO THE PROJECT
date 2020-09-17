## Intsructions to connect Wi-Fi to BBB.
### Note: You will need Internet connection to get through these steps. Please refer **Share WiFi over Ethernet** on this same directory.
- Initially check if you have already installed a Wi-Fi driver in your BBB. (probably it is not.) </br>
- Open terminal and type ***iwconfig*** to check if you can see ***wlan0***.</br>
- If you dont have one, it does mean you've no Wi-Fi driver installed.
- Check online if the Wi-Fi adapter you are using has drivers for linux.
### ***T2U Nano*** Wi-fi adpater driver Installation.
- Connect ethernet to your BBB to access the internet.
- Download the latest kernel-headers of your BBB.</br>
```
 $ sudo apt install linux-headers-$(uname -r)
```
- Update and Upgrade you BBB.</br>
```
$ sudo apt update && sudo apt upgrade**
```
- Do as follows</br>
```
   $ sudo apt install dkms
   $ git clone -b v5.6.4.2 https://github.com/aircrack-ng/rtl8812au.git
   $ cd rtl8812au
   $ sudo ./dkms-install.sh  >>usually takes a long time to complete, be patient.
   $ sudo reboot
```
- After reboot, SSH to you BBB.
- Open terminal and type ***iwconfig*** to check if you can see ***wlan0***.</br>
- if yes, you've successfully installed the drives Cheers!</br>
### Connect BBB to a Wi-Fi nearby network
- Do as follows </br>
```
$ sudo connmanctl (this is already installed in your BBB)

$ connmanctl> agent on
Agent Registerd.

$ connmanctl> scan wifi
Scan completed for wifi.

$ connmanctl> services
```
You will see a list of available Wi-Fi nearby with their SSID name and its ID tag.
```
$ connmanctl> connect wifi_d066842gfjfk_48545erfd_managed_psk

Passphrase? •••••••• type the password</br>
connected wifi_d066842gfjfk_48545erfd_managed_psk
```
- You've successfully connected ping 8.8.8.8 to check the connection.
