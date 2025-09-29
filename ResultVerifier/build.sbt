ThisBuild / version := "0.1.0-SNAPSHOT"

ThisBuild / scalaVersion := "2.12.20"

lazy val root = (project in file(".")).settings(name := "ResultVerifier")

assembly / assemblyJarName := "ResultVerifier.jar"

libraryDependencies += "org.scala-lang.modules" %% "scala-xml" % "2.2.0"
libraryDependencies ++= Seq(
    "com.fasterxml.jackson.core" % "jackson-databind" % "2.19.1",
    "com.fasterxml.jackson.module" %% "jackson-module-scala" % "2.19.1",
    "commons-cli" % "commons-cli" % "1.3.1"
)

assembly / assemblyMergeStrategy := {
    case PathList("META-INF", "versions", xs@_*) => MergeStrategy.discard
    case x =>
        val oldStrategy = (assembly / assemblyMergeStrategy).value
        oldStrategy(x)
}