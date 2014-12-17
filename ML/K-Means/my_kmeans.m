function [SSE, classified_cluster] = my_kmeans(X, cluster, i)
k = 2;
stop = 0;
is_excuted = 0;
while stop == 0
  % fprintf('\nhere.\n\n');
  is_excuted = is_excuted + 1;
  centroid_idxs = initCentroids(cluster);
  centroids(1,:) = X(centroid_idxs(1),:);
  centroids(2,:) = X(centroid_idxs(2),:);
  [idx, SSE] = closestCentroids(X, cluster, centroids);
  centroids = computeCentroids(X, cluster, idx, k);
  if is_excuted == 1
    saved_centroids = centroids;
	% fprintf('\nK-Means Done.\n\n');
	is_excuted = 2;
  else
	if isequal(centroids, saved_centroids) == 1
		stop = 1;
	end
	saved_centroids = centroids;
  end 
end
classified_cluster = i*2 - 2 +idx;
end