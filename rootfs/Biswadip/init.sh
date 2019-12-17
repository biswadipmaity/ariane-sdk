/etc/init.d/S40fixup start
sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/g' /etc/ssh/sshd_config
sed -i 's/#PermitEmptyPasswords no/PermitEmptyPasswords yes/g' /etc/ssh/sshd_config
/etc/init.d/S50sshd restart
/sbin/udhcpc -v eth0
ifconfig eth0 | grep "inet " | awk -F'[: ]+' '{ print $4 }'