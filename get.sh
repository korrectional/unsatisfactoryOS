# this private script get this to from my "local cloud"
source ./.env
cd ../
rm -r osdev
sshpass -p $SSH_PASSWORD scp -r $SSH_ADRESS:/home/david/osdev osdev
cd osdev
echo "Files downloaded"
