package dynagox.experiments

import dynagox.experiments.system.{DBToaster, ExperimentSystem, FIVM}

object Main {
    def main(args: Array[String]): Unit = {
        val (query, systems) = MainOptions.parse(args)
        println(s"Verifying query ${query}.")

        systems.foreach(s => s.load())
        val optReferenceSystem = chooseReferenceSystem(systems)
        if (optReferenceSystem.isEmpty) {
            println("Error: Cannot find any reference system.")
            System.exit(1)
        }

        val referenceSystem = optReferenceSystem.get
        println(s"Pick ${referenceSystem.getName()} as reference system.")

        for (system <- systems) {
            if (system != referenceSystem) {
                val isPassed = verify(referenceSystem, system)
                val status = if (isPassed) "passed" else "failed"
                val systemName = system.getName()
                println(s"System: ${systemName} - Status: ${status}.")

                if (!isPassed)
                    System.exit(1)
            }
        }

        val systemNames = systems.map(_.getName()).mkString(", ")
        println(s"Verified query ${query} for systems ${systemNames}.")
        println("")
    }

    def chooseReferenceSystem(systems: List[ExperimentSystem]): Option[ExperimentSystem] = {
        systems.find(_.isInstanceOf[FIVM]) match {
            case Some(s) => Some(s)
            case None => systems.find(_.isInstanceOf[DBToaster])
        }
    }

    def verify(reference: ExperimentSystem, system: ExperimentSystem): Boolean = {
        val referenceResult = reference.getResult()
        val systemResult = system.getResult()

        if (referenceResult.size != systemResult.size) {
            println(s"Size mismatch: ${reference.getName()} has ${referenceResult.size} rows, ${system.getName()} has ${systemResult.size} rows.")
            return false
        }

        for ((tuple, referenceAnnotations) <- referenceResult) {
            systemResult.get(tuple) match {
                case Some(annotations) =>
                    if (annotations.size != referenceAnnotations.size) {
                        println(s"Mismatch for tuple: (${tuple}):")
                        val s1 = referenceAnnotations.mkString("(", ",", ")")
                        val s2 = annotations.mkString("(", ",", ")")
                        println(s"${reference.getName()}: ${s1}")
                        println(s"${system.getName()}: ${s2}")
                        return false
                    }

                    for (i <- referenceAnnotations.indices) {
                        val referenceAnnotation = referenceAnnotations(i)
                        val annotation = annotations(i)

                        if (system.isApproximated()) {
                            if (annotation < (referenceAnnotation - 0.01) || annotation > (referenceAnnotation * 1.1 + 0.01)) {
                                println(s"Mismatch for tuple: (${tuple}):")
                                println(s"${reference.getName()}: ${referenceAnnotation}")
                                println(s"${system.getName()}: ${annotation}")
                                return false
                            }
                        } else {
                            if (Math.abs(referenceAnnotation - annotation) > 0.01) {
                                println(s"Mismatch for tuple: (${tuple}):")
                                println(s"${reference.getName()}: ${referenceAnnotation}")
                                println(s"${system.getName()}: ${annotation}")
                                return false
                            }
                        }
                    }
                case None =>
                    println(s"Tuple (${tuple}) not found in ${system.getName()}")
                    return false
            }
        }

        true
    }
}
