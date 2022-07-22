<?php
require 'db_connect.php';

// Where the file is going to be placed 
$target_path = "C:/elpe/";

/* Add the original filename to our target path. Result is "uploads/filename.extension" */
 $target_path = $target_path . basename( $_FILES['uploadedfile']['name']); 
// $_FILES['uploadedfile']['tmp_name'];  
// print $target_path;

// ------------------------------------------------------------
 echo '<br>';
 echo 'file = ',$_FILES['uploadedfile']['tmp_name'],'<br>';
 echo 'file = ',$_FILES['uploadedfile']['name'],'<br>';
 echo 'file = ',$_FILES['uploadedfile']['tmp_name'],'<br>';
 echo 'Target path = ',$target_path,'<br><br>';
 
 // $fname = $target_path . '/' . $_FILES['uploadedfile']['name'];
 $fname = $target_path;
 echo 'will copy to  file ',$fname,'<br><br>';

if(move_uploaded_file($_FILES['uploadedfile']['tmp_name'], $target_path )) {
    echo "The file ".  basename( $_FILES['uploadedfile']['name']). 
    " has been uploaded<br>";
//	print_r($_FILES);
	echo '<br>';
	

//	print_r($n1lines);
	echo '<br>';
	$db1 = elpe2_connect();
	if(!$db1) {
		echo 'could not connect to database<br>';
	} else {
		echo 'database connected!<br>';
	};

//	$fname = 'C:\elpe\php.ini';
	$handle = fopen($fname,"r");
	echo 'opened handle of ',$fname,' is :[',$handle,']<br>';
	$total=0;
	
	while (($data = fgetcsv($handle, 1000, ";")) !== FALSE) {
    	$num = count($data);
    	
    	echo "Line $row $num fields: (";
    	$row++;
    	if($row>34) break;
    	// show the line
    	for ($c=0; $c < $num; $c++) {
        	echo '[',$data[$c] . "]";
        };
    	echo ')<br>';

    	// import the line in database after parsing
    	
    	if($data[0]>0 && $data[0]<25) { // we have an hour
    		// convert Greek numeric value to latin1
    		$ng = str_replace(',','.',$data[1]);
//    		print "recoded numeric = [$ng]<br>";
				
//    		print "num = $n1<br>";
    		$total = $total+$ng;
		} else {
			if($data[0]=='date') { $date1=$data[1]; echo "found date = $date1<br>";};
		}
		
	};
	print "total = $total<br>";
	fclose($handle);
	
} else{
    echo "There was an error uploading the file, please try again!";
	echo 'Error = ',$_FILES['uploadedfile']['error'],'<br>';
	echo 'Name = ',$uploadedfile_name,'--<br>';
	print_r($_FILES);
}

?>
