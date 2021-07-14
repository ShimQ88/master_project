#parameters
cls=0
mode=0
path_of_file=/home/kyu/Desktop/birds
idx_file_name=/index.txt
idx=output_the_first_contour_yolo/birds/index.txt

# echo $idx
./main -mode $mode --cls $cls --idx $idx --data $path_of_file

# ./main -mode 0 --cls 0 --idx "ROI_images/index.txt" --data "/home/kyu/Desktop/rodents_data"

#read index
value=`cat output_the_first_contour_yolo/birds/index.txt`

#read total number of item in directory
total=`ls $path_of_file -l | wc -l`

echo $total

total=`expr $total - 5`
#loop work until index number over total number of item in directory
while [ $value -le $total ]
do
	
	./main -mode $mode --cls $cls --idx $idx --data $path_of_file
	value=`cat output_the_first_contour_yolo/birds/index.txt`
done
echo "Terminate bash programming"