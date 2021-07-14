path_yolov5=`cat extra_script_for_the_second_cycle/path_yolov5.txt`
file_name=`cat extra_script_for_the_second_cycle/file_name.txt`

echo $file_name

python3 "$path_yolov5/detect.py" --source $file_name  --weights "$path_yolov5/weights/last.pt" --conf 0.4