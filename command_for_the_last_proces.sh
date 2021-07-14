#parameters
mode=2
# path_of_file=/home/kyu/Desktop/fd_yolo_data_collection/output_the_second_contour_yolo/rodents_data
# path_of_file=output_the_second_contour_yolo/rodents_data
# path_of_file=output_the_second_contour_yolo/birds
# path_of_file=output_the_second_contour_yolo/rt_img
path_of_file=output_the_second_contour_yolo/bt_img
# idx=output_the_final_contour_yolo/rodents_data/index.txt
# idx=output_the_final_contour_yolo/rodents_data/index.txt
# idx=output_the_final_contour_yolo/birds/index.txt
# idx=output_the_final_contour_yolo/rt_img/index.txt
idx=output_the_final_contour_yolo/bt_img/index.txt
# number_count=/home/kyu/Desktop/fd_yolo_data_collection/output_the_second_contour_yolo/rodents_data/contour
# number_count=output_the_second_contour_yolo/rodents_data/contour
# number_count=output_the_second_contour_yolo/birds/contour
# number_count=output_the_second_contour_yolo/rt_img/contour
number_count=output_the_second_contour_yolo/bt_img/contour
# echo $idx
./main -mode $mode --idx $idx --data $path_of_file

# ./main -mode 0 --cls 0 --idx "ROI_images/index.txt" --data "/home/kyu/Desktop/rodents_data"

#read index
# value=`cat output_the_final_contour_yolo/rodents_data/index.txt`
# value=`cat output_the_final_contour_yolo/rodents_data/index.txt`
# value=`cat output_the_final_contour_yolo/birds/index.txt`
# value=`cat output_the_final_contour_yolo/rt_img/index.txt`
value=`cat output_the_final_contour_yolo/bt_img/index.txt`

#read total number of item in directory
total=`ls $number_count -l | wc -l`

echo $total

total=`expr $total - 2`
#loop work until index number over total number of item in directory
while [ $value -le $total ]
do
	
	./main -mode $mode --idx $idx --data $path_of_file
	# value=`cat output_the_final_contour_yolo/rodents_data/index.txt`
	# value=`cat output_the_final_contour_yolo/rodents_data/index.txt`
	# value=`cat output_the_final_contour_yolo/birds/index.txt`
	# value=`cat output_the_final_contour_yolo/rt_img/index.txt`
	value=`cat output_the_final_contour_yolo/bt_img/index.txt`
done
echo "Terminate bash programming"