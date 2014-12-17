filename = 'CompleteImageFeatures.csv';
X = xlsread(filename); %read the data from file
K = 4; %number of clusters
number_of_iterations = 115; %number of iterations
idx = ones(size(X,1),1);
temp_idx = idx;
number_of_clusters = 1;
run_time = 0;
while number_of_clusters < K 
run_time = run_time + 1;
for i=1:number_of_clusters
	selected_cluster = [];
	for j=1:size(X,1)
		if temp_idx(j) == i
			selected_cluster = [selected_cluster ; j];
		end		
	end
	fprintf('\nselected_cluster.\n\n');
	selected_cluster'
	min = flintmax;
	for j=1:number_of_iterations
		[SSE, new_idx] = my_kmeans(X, selected_cluster, i); %bisect the selected cluster
		if SSE < min 
			saved_idx = new_idx;
			min = SSE;
		end
	end
	fprintf('\nsaved_idx.\n\n');
	saved_idx'
	temp_idx = idx;
	for k=1:size(selected_cluster,1)
		idx(selected_cluster(k)) = saved_idx(k);  %add the two clusters from the bisection
	end
	if run_time == 1
		temp_idx = idx;
	end
	  % fprintf('\nK-Means Done.\n\n');
end
fprintf('\nidx.\n\n');
xlswrite('resultKMeans.csv',idx,'resultKMeans','B1');
number_of_clusters = number_of_clusters*2;
end

