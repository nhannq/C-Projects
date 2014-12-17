function centroid_idxs = initCentroids(cluster)
centroid_idxs = [];
centroid1 = randi(size(cluster,1));
centroid_idxs = [centroid_idxs centroid1];
centroid2 = randi(size(cluster,1));
while centroid2 == centroid1 
	centroid2 = randi(size(cluster,1));
end
centroid_idxs = [centroid_idxs centroid2];
end