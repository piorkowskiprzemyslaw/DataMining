#!/usr/bin/awk -f
# Parse ./DataMining output in order to calculate the Geometric Mean of Precission and Recall Values

BEGIN {
	RS="== Split "
	FS="\n"

	score["precission"]=0
	score["recall"]=0
	precission_ln_sum=0
	recall_ln_sum=0
	cnt=0
}

{
	gsub(/[^0-9;]*: / , "", $4) #remove all chars but numbers&semicolon from Quality_field
	split($4, quality_fields, ";")
	gsub(/[^0-9*]/, "", $3) #leave only numbers in Attributes_Field
	if (length(quality_fields[6]) > 0) #eliminate null records
	{
		#print "Parsed:", quality_fields[6], quality_fields[7]
		precission_ln_sum += log(quality_fields[6])
		recall_ln_sum += log(quality_fields[7])
		cnt++
	}
}
END {
	score["precission"] = exp(precission_ln_sum/cnt)
	score["recall"] = exp(recall_ln_sum/cnt)
	printf "Attributes: %d; Precission: %f; Recall: %f\n", $3, score["precission"], score["recall"]
}
