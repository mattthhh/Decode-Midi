curl https://irma-web1.math.unistra.fr/math-musique/midi.html --output home.html
xmllint --html --xpath "//td/a/h3" home.html > cat.html 
sed 's/<[^>]*>/\n/g' cat.html > categories.txt
sed '/^$/d' categories.txt > temp
mv temp categories.txt
tr '[:upper:]' '[:lower:]' < categories.txt > categories_l.txt 


