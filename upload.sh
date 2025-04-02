# this private script uploads this to my "local cloud"
source ./.env
cd ../
sshpass -p $SSH_PASSWORD ssh $SSH_ADRESS "rm -rf osdev"
sshpass -p $SSH_PASSWORD scp -r osdev $SSH_ADRESS:/home/david/osdev
cd osdev
echo "Files uploaded"