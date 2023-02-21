cp ./DecodeMidi /Applications/DecodeMidi.app/Contents/MacOS/DecodeMidi
cp drop.png /Applications/DecodeMidi.app/Contents/Resources/drop.png
rm -rf DecodeMidi.dmg
create-dmg --icon "/Applications/DecodeMidi.app" 0 0 --app-drop-link 0 200 DecodeMidi.dmg /Applications/DecodeMidi.app
