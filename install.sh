#!/bin/bash

clear
echo "==============="
echo "Raspi as Acces Point"
apt update
apt install -y hostapd dnsmasq
DEBIAN_FRONTEND=noninteractive apt install -y netfilter-persistent iptables-persistent
systemctl unmask hostapd.service
systemctl enable hostapd.service
cat dhcpcd.conf >> /etc/dhcpcd.conf
echo "net.ipv4.ip_forward=1" > /etc/sysctl.d/routed-ap.conf
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
netfilter-persistent save
mv /etc/dnsmasq.conf /etc/dnsmasq.conf.BAK
cp dnsmasq.conf /etc/dnsmasq.conf
cp hostapd.conf /etc/hostapd/hostapd.conf
