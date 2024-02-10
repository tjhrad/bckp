#!/bin/bash

# A simple example of how to use bckp in a bash script.
bin/bckp -i /home/quasar -o /run/media/quasar/c6648242-309f-4363-bf9b-dad2debad88b/arch_linux_backup/home_quasar -l ./test_logs/test_log.txt
bin/bckp -i /run/media/quasar/c6648242-309f-4363-bf9b-dad2debad88b -o /run/media/quasar/Storage_Backup/Linux_Storage_Backup -l ./test_logs/test_log2.txt