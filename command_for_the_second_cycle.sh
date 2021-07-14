#parameters
# cls=0
cls=1
mode=4
path_of_file=/media/q/q_drive/rodents_data
# path_of_file=/media/q/q_drive/birds
idx_file_name=/index.txt
idx=the_second_cycle/rodents_data/index.txt
# idx=the_second_cycle/birds/index.txt
path_yolo=~/Desktop/yolo/
contour_txt=contour.txt
# echo $idx
./main -mode $mode --cls $cls --idx $idx --data $path_of_file --path_yolov5 $path_yolo --contour $contour_txt

# ./main -mode 0 --cls 0 --idx "ROI_images/index.txt" --data "/home/kyu/Desktop/rodents_data"

#read index
value=`cat the_second_cycle/rodents_data/index.txt`
# value=`cat the_second_cycle/birds/index.txt`

#read total number of item in directory
total=`ls $path_of_file -l | wc -l`

echo $total

total=`expr $total - 5`
#loop work until index number over total number of item in directory
while [ $value -le $total ]
do
	
	./main -mode $mode --cls $cls --idx $idx --data $path_of_file
	value=`cat the_second_cycle/rodents_data/index.txt`
	# value=`cat the_second_cycle/birds/index.txt`
done
echo "Terminate bash programming"