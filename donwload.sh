function sed_p() {
    sed "$1" $2 > temp
    mv temp $2
}

curl -s https://math-musique.pages.math.unistra.fr/midi.html --output home.html
xmllint --html --xpath "//td/a/h3" home.html > cat.html 2> /dev/null
sed 's/<[^>]*>/\n/g' cat.html > categories.txt
sed '/^$/d' categories.txt > temp
mv temp categories.txt
tr '[:upper:]' '[:lower:]' < categories.txt > categories_l.txt 
# remove ',' character
sed 's/,//g' categories_l.txt > temp
mv temp categories_l.txt
sed 's/hard rock/hardrock/g; s/heavy //g; s/musique de //g; s/é/e/g; s/ç/c/g; s/&amp; //g' categories_l.txt > temp
mv temp categories_l.txt
sed 's/ /-/g' categories_l.txt > temp
mv temp categories_l.txt

mkdir songs
mkdir html
cd html
cat ../categories_l.txt | while read line 
do
	echo "... Downloading $line ..."
	curl -s https://math-musique.pages.math.unistra.fr/midi/$line-midi.html --output "$line.html"
	xmllint --html --xpath "//a[not(ancestor::ul[@class='menu'])]/@href" "$line.html" > temp 2> /dev/null
	sed 's/href="//g; s/"/\n/g; s/ //g' temp > temp2
	mv temp2 temp
	sed -e :a -e '$d;N;2,2ba' -e 'P;D' temp > temp2
	mv temp2 temp
	cd ../songs
	mkdir $line
	cd $line
	cat ../../html/temp | while read -r line2
	do
		# get after last '/'
		TITLE=${line2##*/}
		echo $TITLE
		curl https://math-musique.pages.math.unistra.fr/midi/$line2 --output "$TITLE"
	done
	cd ../../html
	echo "--- Downloading $line done ---"
done
cd ../songs
rm -rf */*.pdf */*.html
for dir in */
do
    echo "Cleaning up MIDI files in $dir ..."
    find "$dir" -type f -name "*.mid" | while read -r file
    do
        if cat "$file" | grep -q html; then
            echo "Deleting $file ..."
            rm "$file"
        fi
    done
done

