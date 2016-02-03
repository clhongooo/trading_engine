#!/bin/bash
CURDIR=$(pwd)

printModifiedTime() {

  local RED="\e[31m"
  local GREEN="\e[32m"
  local NOCOLOR="\e[0m"

  TRDGENGLOC=$CURDIR/Debug/TradingEngine
  echo "--------------------------------------------------"
  if [[ -f $TRDGENGLOC ]]
  then
      echo -e "["$GREEN"success"$NOCOLOR"] $TRDGENGLOC was built"
      ls -lh $TRDGENGLOC
  else
      echo -e "["$RED"error"$NOCOLOR"] Cannot build $TRDGENGLOC"
  fi
  echo "--------------------------------------------------"
}

printModifiedTime

while true
do
    cd $CURDIR
    change=$(inotifywait -e close_write,moved_to,create ../NirvanaCommon/ATU ../NirvanaCommon/Logger ../NirvanaCommon/Math ../NirvanaCommon/Model ../NirvanaCommon/SharedObjects ../NirvanaCommon/Util ../NirvanaInfrastructure/DataAggregation ../NirvanaInfrastructure/Facilities ../NirvanaInfrastructure/LowPriorityFacilities ../NirvanaInfrastructure/OrderExecutionAlgo ../NirvanaInfrastructure/PortfolioGeneration ../NirvanaInfrastructure/ProbDistributionGeneration ../NirvanaInfrastructure/SharedObjects ../NirvanaInfrastructure/TechIndUpdating ../NirvanaInfrastructure/VolSurfaceCalculation)
    change=${change#./ * }
    if [[ -n "$change" ]]
    then
        cd $CURDIR/../NirvanaCommon/Debug && rm -f *.a
        make all
        cd $CURDIR/../NirvanaInfrastructure/Debug && rm -f *.a
        make all
        cd $CURDIR/Debug && rm -f TradingEngine && make clean && make all

        printModifiedTime
    fi
done
