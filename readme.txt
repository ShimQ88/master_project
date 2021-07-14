This is the Demo instruction and code  

/******************************************************/
/******************************************************/
/**************A Semi-Automate Labeling****************/
/******************************************************/
/******************************************************/

Example Video: https://youtu.be/08csQzL1UZQ

1) command: ./main -mode 0 --cls 0 --data test_dataset_rodents --idx index.txt
	
	(result files in: output_the_first_contour_yolo)

2) command (Filtering 1): ./main -mode 1 --data output_the_first_contour_yolo/test_dataset_rodents  --idx index.txt
	
	(result files in: output_the_second_contour_yolo)
	Choosing the index numbers want to save in

	e.g) Click the window and Press "enter" button.
		 Writing Index numbers such as 8,9,10,11,41,45 
		 and then "y"

3) command (Filtering 2): ./main -mode 2 --data output_the_second_contour_yolo/test_dataset_rodents  --idx index.txt

	(result files in: output_the_final_contour_yolo)
	Choosing the index numbers want to "not!!" save in

	e.g) Enter
         Click the window and Press "enter" button. 
		 "s"<- which is skip
	
	*note* 
	Sometimes the third command shows the result segmentation fault.
	Ff it happen, try the third command again. 


Final Results:

the "contour.txt" file is the training resource in FDs.
The files in a "yolo" folder is the training resource in Yolo.

/******************************************************/
/******************************************************/
/******************************************************/


/******************************************************/
/******************************************************/
/**************Training Labeled Data in FDs************/
/******************************************************/
/******************************************************/

Example Video:https://youtu.be/LMtrj1llQX4

command: ./main -mode 3 --data contour_first_second_third_v2.txt --cls 2 --per 0.9999 --tech 2

Training resource: contour_first_second_third_v2.txt(5666 CEs).
Training Algorithm: Random Forest.
Final Result: weight/rf_demo.xml

/******************************************************/
/******************************************************/
/******************************************************/

/******************************************************/
/******************************************************/
/*****************Detection Using FDs******************/
/******************************************************/
/******************************************************/

Commands:
In the best environment:
Bird1)
./main -mode 6 --source test_videos_best_environment/b1.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Bird2)
./main -mode 6 --source test_videos_best_environment/b2.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Bird3)
./main -mode 6 --source test_videos_best_environment/b3.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Rodent1)
./main -mode 6 --source test_videos_best_environment/r1.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Rodent2)
./main -mode 6 --source test_videos_best_environment/r2.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Rodent3)
./main -mode 6 --source test_videos_best_environment/r3.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2


In the random environment:
Bird1)
./main -mode 6 --source test_vidoes_random_environment/b1.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Bird2)
./main -mode 6 --source test_vidoes_random_environment/b2.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Bird3)
./main -mode 6 --source test_vidoes_random_environment/b3.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Rodent1)
./main -mode 6 --source test_vidoes_random_environment/r1.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Rodent2)
./main -mode 6 --source test_vidoes_random_environment/r2.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2

Rodent3)
./main -mode 6 --source test_vidoes_random_environment/r3.avi --weight weight/rf_demo.xml --tag tag.txt --tech 2



Real-time:
./main -mode 6 --weight weight/rf_demo.xml --tag tag.txt --tech 2
Example Video:https://youtu.be/LMtrj1llQX4
/******************************************************/
/******************************************************/
/******************************************************/
