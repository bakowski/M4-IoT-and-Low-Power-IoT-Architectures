import gc
import http.client
import micropython
gc.collect()


micropython.mem_info()
import wifista

wifista.connect()

conn = http.client.HTTPConnection("http://api.thingspeak.com/update?api_key=api_key=YOX31M0EDKO0JATK&field1=33.3&field2=55.5")
#conn.request("HEAD","/info.php?request = sam")
res = conn.getresponse()
print (res.status, res.reason)
# Result:
print("200 OK")
print("The pastebin URL is:%s"%pastebin_url)

