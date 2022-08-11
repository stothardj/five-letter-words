/^[a-z]{5}$/ {
	n=split($0,arr,"")
	asort(arr)
	dup=0
	prev=arr[1]
	for(i=2; i<=n; i++) {
		dup = dup || (prev == arr[i])
		prev=arr[i]
	}
	if (!dup) print
}

