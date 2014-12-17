function [idx, SSE] = closestCentroids(X, cluster, centroids)

idx = zeros(size(cluster,1), 1);
SSE = 0;
m = size(cluster,1);
for i=1:m
	min = sqrt(sum((X(cluster(i),:)-centroids(1,:)).^2));
	SSE = SSE + min;	
	idx(i) = 1;
	temp = sqrt(sum((X(cluster(i),:)-centroids(2,:)).^2));
    if temp < min 
      idx(i) = 2;
	  SSE = SSE - min + temp;
    end;
end;

end