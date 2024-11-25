package main

import "github.com/gin-gonic/gin"

func main() {
	g := gin.Default()
	g.StaticFile("/", "../demo/asset/index.html")
	g.Run("127.0.0.1:8080")
}
