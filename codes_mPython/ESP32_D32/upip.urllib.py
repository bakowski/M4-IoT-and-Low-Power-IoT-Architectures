
    import network
    import upip
    
    ssid      = "Livebox-08B0"
    password  =  "G79ji6dtEptVTPWmZP"
 
    station = network.WLAN(network.STA_IF)
    station.active(True)
    station.connect(ssid,password)

  upip.install('urllib')
    

 