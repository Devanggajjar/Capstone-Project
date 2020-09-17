# Script to auto-update commits

echo "Pulling the remote repository..."
git pull origin master >/dev/null 2>&1

sleep 2
echo "Adding Changes....."
git add . >/dev/null 2>&1
git commit -m "New Update" >/dev/null 2>&1

sleep 2
echo "Please Login"
git push origin master

echo -e "\n\nDone!"
