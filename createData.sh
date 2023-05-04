mkdir data
ls songs | while read category 
do
	ls songs/$category | while read song
	do
		echo "--- Doing $category-$song ---"
		./DecodeMidi songs/$category/$song >> data/$category-$song-song.txt
		echo $category >> data/$category-$song-cat.txt
	done
done
