mkdir data
ls songs | while read category 
do
	ls songs/$category | while read song
	do
		touch data/$category-$song-song.txt
		touch data/$category-$song-cat.txt
		echo "--- Doing $category-$song ---"
		./DecodeMidi songs/$category/$song >> data/$category-$song.txt
		echo $category >> data/$category-$song-cat.txt
	done
done
