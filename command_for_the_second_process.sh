#parameters
mode=1
# path_of_file=the_second_cycle/rodents_data
# path_of_file=the_second_cycle/birds
# path_of_file=the_second_cycle/bt_img
path_of_file=the_second_cycle/rt_img
# idx=output_the_second_contour_yolo/rodents_data/index.txt
# idx=output_the_second_contour_yolo/birds/index.txt
# idx=output_the_second_contour_yolo/bt_img/index.txt
idx=output_the_second_contour_yolo/rt_img/index.txt
# number_count=the_second_cycle/rodents_data/contour
# number_count=the_second_cycle/birds/contour
# number_count=the_second_cycle/bt_img/contour
number_count=the_second_cycle/rt_img/contour
# echo $idx
./main -mode $mode --idx $idx --data $path_of_file

# ./main -mode 0 --cls 0 --idx "ROI_images/index.txt" --data "/home/kyu/Desktop/rodents_data"

#read index
# value=`cat output_the_second_contour_yolo/rodents_data/index.txt`
# value=`cat output_the_second_contour_yolo/birds/index.txt`
# value=`cat output_the_second_contour_yolo/bt_img/index.txt
value=`cat output_the_second_contour_yolo/rt_img/index.txt`

#read total number of item in directory
total=`ls $number_count -l | wc -l`

echo $total

total=`expr $total - 2`
#loop work until index number over total number of item in directory
while [ $value -le $total ]
do
	
	./main -mode $mode --idx $idx --data $path_of_file
	# value=`cat output_the_second_contour_yolo/rodents_data/index.txt`
	# value=`cat output_the_second_contour_yolo/birds/index.txt`
	# value=`cat output_the_second_contour_yolo/bt_img/index.txt`
	value=`cat output_the_second_contour_yolo/rt_img/index.txt`
done
echo "Terminate bash programming"