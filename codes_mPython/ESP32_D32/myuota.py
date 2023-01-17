import uota
import machine

from connect_wifi import connect_wifi
connect_wifi()

if do_ota_update and uota.check_for_updates():
      uota.install_new_firmware()
      machine.reset()
      
      