ralf_listen_port=3869
. /etc/clearwater/config
ralf_port=`echo $ralf_hostname | perl -p -e 's/.+:(\d+)/$1/'`
cat << EOF
define command{
        command_name    restart-ralf
        command_line    /usr/lib/nagios/plugins/clearwater-abort \$SERVICESTATE$ \$SERVICESTATETYPE$ \$SERVICEATTEMPT$ /var/run/ralf.pid 30
        }


define service{
        use                             cw-service         ; Name of service template to use
        host_name                       local_ip
        service_description             Ralf HTTP port open
        check_command                   http_ping!$ralf_port
        event_handler                   restart-ralf
        }

define service{
        use                             cw-service         ; Name of service template to use
        host_name                       local_ip
        service_description             Ralf Diameter port open
        check_command                   check_tcp_port!$ralf_listen_port
        event_handler                   restart-ralf
        }

EOF
