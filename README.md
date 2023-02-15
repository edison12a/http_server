
# build & run command

`docker rm -f newhrservice; docker rmi -f newhrservice; docker build -t newhrservice:latest .; docker run -p9090:9090 --name newhrservice -it newhrservice`
