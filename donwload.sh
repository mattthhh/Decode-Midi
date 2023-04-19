function sed_p() {
    sed "$1" $2 > temp
    mv temp $2
}

curl -s https://irma-web1.math.unistra.fr/math-musique/midi.html --output home.html
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

mkdir html
cd html
I=1
cat ../categories_l.txt | while read line 
do
	curl -s https://irma-web1.math.unistra.fr/math-musique/midi/$line-midi.html --output "$line.html"
done
