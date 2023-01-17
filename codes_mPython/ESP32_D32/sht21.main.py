import sht21
with sht21.SHT21(0) as sht21:
    print "Temperature: %s"%sht21.read_temperature()
    print "Humidity: %s"%sht21.read_humidity()

