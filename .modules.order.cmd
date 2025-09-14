cmd_/home/ubuntu/ldd/modules.order := {   echo /home/ubuntu/ldd/msgdriver.ko; :; } | awk '!x[$$0]++' - > /home/ubuntu/ldd/modules.order
