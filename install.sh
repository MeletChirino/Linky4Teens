#!/bin/bash

clear
echo "==============="
echo "Raspi as Acces Point"
sudo apt update
sudo apt install -y hostapd dnsmasq
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
