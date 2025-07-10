#!/bin/bash

#EPO=${1-MTK14.EPO}
# EPO="EPO.DAT"

#EPO=${1-EPO_GPS_3_1.DAT}
# file=${2-$EPO}

# if [ -e $file ] ; then
# 	mv $file $file.last
# fi

#wget -O${file} ftp://gtopagpsenduser01:enduser080807@ftp.gtop-tech.com/${EPO} -t 1 -T 10 || rm -f $file
#wget -O${file} https://10.8.0.1/gps/${EPO} -t 1 -T 10  --no-check-certificate || rm -f $file

#gps only epo
wget -O${file} http://wepodownload.mediatek.com/EPO_GPS_3_1.DAT -t 1 -T 10 || cp $file.last $file

#3 days gps+glonass
#wget -O${file} http://wepodownload.mediatek.com/EPO_GR_3_1.DAT -t 1 -T 10 || cp $file.last $file

#6 hours gps+glonass The size of /home/x_user/gps_utils/MTK14.EPO is not an even multiple of EPO_SET_SIZE(2304).  It may be corrupted.
#wget -O${file} http://wepodownload.mediatek.com/QG_R.DAT  -t 1 -T 10 || cp $file.last $file

#6 hours gps
#wget -O${file} http://wepodownload.mediatek.com/QGPS.DAT -t 2 -T 10 || cp $file.last $file 
#mv ./EPO_GPS_3_1.DAT ./MTK14.EPO
#cp ./MTK14.EPO.last ./MTK14.EPO

# #!/bin/bash

# # Serial port configuration
# PORT="/dev/ttymxc1"
# BAUD="115200"

# # Configure the serial port
# stty -F $PORT $BAUD cs8 -parenb -cstopb

# # Send the command to enable GGA and RMC sentences
# echo -ne "\$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n" > $PORT

# # Wait for the GPS to process the command
# sleep 1

# # Read the response
# timeout 5 cat $PORT
