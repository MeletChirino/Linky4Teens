#!/bin/bash

clear
echo "=================================="
echo "Installing Programs"
sudo apt update
sudo apt install -y apache2 libapache2-modwsgi-py3 hostapd dnsmasq python3 python3-pip python3-venv libtiff5 libopenjp2-7 libatlas-base-dev


echo "==============="
echo "Raspi as Acces Point"
sudo DEBIAN_FRONTEND=noninteractive apt install -y netfilter-persistent iptables-persistent
sudo systemctl unmask hostapd.service
sudo systectl enable hostapd.service
sudo cat dhcpcd.conf >> /etc/dhcpcd.conf
sudo echo "net.ipv4.ip_forward=1" > /etc/sysctl.d/routed-ap.conf
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo netfilter-persistent save
sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.BAK
sudo cp dnsmasq.conf /etc/dnsmasq.conf
sudo cp hostapd.conf /etc/hostapd/hostapd.conf
echo "Raspberry Configured as Access Point, type something to continue"

echo "==============="
echo "Installing Virtual Environment"
echo "Creating virtual environment"
python3 -m venv ./venv
echo "Activating Venv"
source venv/bin/activate
echo "installing libraries"
pip3 install -r requirements.txt
echo "virtual environment installed type something to continue"

echo "==============="
echo "Setting up apache"
sudo cp linky4teens.conf /etc/apache2/sites-available/linky4teens.conf
sudo a2dissite 000-default
sudo a2ensite linky4teens
echo "Reloading apache"
sudo systemctl reload apache2
sudo chown -R :www-data new_gui/media/
sudo chmod -R 775 new_gui/media/
sudo chmod 775 new_gui/
echo "Apache2 installed type something to finish"
