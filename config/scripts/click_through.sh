#!/bin/sh

LICENSE="PATENT"

if [ -z $PAGER ] ; then
    PAGER=more
fi

$PAGER $LICENSE
echo -n "Do you agree to the terms of this license [y/n]?"
read
if [ $REPLY != y ] ; then
    echo -n "You have no license to run RTLinux."
    exit -1
fi

rm -f .rtlinux_license_tmp
echo -n '/proc/version: ' >> .rtlinux_license_tmp
cat /proc/version >> .rtlinux_license_tmp
echo -n 'uname: ' >> .rtlinux_license_tmp
uname -a >> .rtlinux_license_tmp
if [ ! -z "$1" ] ; then
  echo RTLinux: $1 >> .rtlinux_license_tmp
fi
echo -n '/etc/issue: ' >> .rtlinux_license_tmp
cat /etc/issue >> .rtlinux_license_tmp

cat <<EOF

--------------------------------------------------------------
To activate the license please email license@fsmlabs.com with:
mail -s "License"  license@fsmlabs.com < .rtlinux_license_tmp 
--------------------------------------------------------------

EOF

#rm -f .rtlinux_license_tmp

touch ${RTL}/.rtlinuxaccepted

exit 0
