nodemail
=======

this is the mail agent. To use it you need to launch with ncw https://github.com/nodecast/ncw
NCW will connect to your NCS instance. nodemail is waiting for a payload which is a json like this :

```bash
{ "exp": "user@domain.tld", "dest": "dest@domain.tld", "object": "your object", "body": "your body" }
```


## INSTALL

first, install some debs :


```bash
sudo apt-get install g++ make autoconf libboost-all-dev libqt4-dev
```

next, get submodule libraries :

```bash
git submodule update --init
```


make the libqxt library

```bash
cd externals/libqxt
./configure
make
sudo make install
```


make nodemail :

```bash
qmake
make
mkdir $HOME/bin
cp nodeftp $HOME/bin
sudo mkdir /var/lib/nodemail
sudo chown $USER:$USER /var/lib/nodemail
```

## USE

Create a node with the NCS web interface or the HTTP API ( https://github.com/nodecast/ncs )

and launch the service

```bash
ncw --worker-type=service --worker-name=mail --ncs-ip=NCS_IP --ncs-port=5569 --node-uuid=nodeuuid --node-password=nodepassword --stdout=true --exec="nodemail --smtp-hostname=mail.service.tld--smtp-username=user@hostname.com --smtp-password=userpassword"
```

PUSH a payload to this service

```bash
curl -H "X-user-token: user-token" -H "X-node-uuid: node-uuid" -H "X-node-password: node-password"  -H "X-payload-type: mail" -H "X-payload-mime: json" -H "X-workflow-uuid: workflow-uuid" -H "X-payload-action: push" -d '{ "exp": "user@domain.tld", "dest": "dest@domain.tld", "object": "your object", "body": "your body" }' http://ZEROGWIP:8000/payload
```
