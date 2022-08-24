#!/usr/bin/env bash

source ./setting.sh

ENV="none"
TIMESTAMP=$(date +%Y.%m)
PATH="../../build/$ENV"

echo "Environment :: " "$ENV"
echo "Path        :: " "$PATH"
echo "Build       :: " "$TIMESTAMP-$ENV"

sleep 3

docker build -t cpd9957/named-data-networking:"$TIMESTAMP"-$ENV $PATH

sleep 1

echo "Docker image build completed..."

sleep 3

apt-get update; apt-get install -y docker-compose

docker-compose stop; docker-compose down; docker-compose rm -f; docker network node_network -f; docker-compose build; docker-compose up -d; docker volume node_volume -f; docker-compose logs -f >> $ENV.log& date
