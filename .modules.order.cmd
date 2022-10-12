cmd_/media/josh/ESD-USB/test2/modules.order := {   echo /media/josh/ESD-USB/test2/memdrv.ko; :; } | awk '!x[$$0]++' - > /media/josh/ESD-USB/test2/modules.order
