import urllib3
import json

http = urllib3.PoolManager()
r = http.request('GET', 'https://api.cardmarket.com/ws/v1.1/account', headers={
    
})

print(r.status)

print(r.data)

print(r.headers)

print(r.json())