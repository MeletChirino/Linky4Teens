#!/bin/bash

clear
echo "==============="
echo "Raspi as Acces Point"
sudo apt update
sudo apt install -y hostapd dnsmasq python3 python3-pip python3-venv libtiff5 libopenjp2-7 libatlas-base-dev

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
read x

echo "==============="
echo "Installing Virtual Environment"
python3 -m venv ./venv
source venv/bin/activate
pip3 install -r requierements.txt
echo "virtual environment installed type something to continue"
read x
