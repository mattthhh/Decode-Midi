mkdir data
ls songs | while read category 
do
	ls songs/$category | while read song
	do
		touch data/$category-$song.txt
		echo "--- Doing $category-$song ---"
		./DecodeMidi songs/$category/$song >> data/$category-$song.txt
		echo "Style : $category" >> data/$category-$song.txt
	done
done
