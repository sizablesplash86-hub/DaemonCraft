rm /usr/sbin/daemoncraft
gcc src/*.c -o /usr/sbin/daemoncraft -lcurl
sudo daemoncraft -m
