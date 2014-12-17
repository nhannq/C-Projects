function centroids = computeCentroids(X, cluster, idx, k)
m = size(cluster,1);
n = size(X,2);
centroids = zeros(k, n);

count = zeros(k,1);
for i=1:m
    centroids(idx(i),:) = centroids(idx(i),:) + X(cluster(i),:);
    count(idx(i)) = count(idx(i)) + 1;    
end;

for i = 1:k
    count(i);
    centroids(i,:) = centroids(i,:)./count(i);
end;
end
