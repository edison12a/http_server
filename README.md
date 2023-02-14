
# build & run command

`docker rm newhrservice; docker rmi newhrservice; docker build -t newhrservice:latest .; docker run -p9090:9090 --name newhrservice -it newhrservice`
