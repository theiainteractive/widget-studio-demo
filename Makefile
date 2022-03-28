env = "local"
ue = ""
version = ""
target_platform = ""
nobuild = "false"
SHELL 		:= /usr/bin/env bash
.SHELLFLAGS := -eo pipefail -c
.DEFAULT = help

help:
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make \n\nTargets:\n"} /^[a-zA-Z_-]+:.*?##/ { printf "\033[36m%-10s\033[0m %s\n", $$1, $$2 }' $(MAKEFILE_LIST)

.PHONY: setup
setup: ## Sets up the repo from its template state
	@echo "Setting up repo from boilerplate..."
	@./bin/setup.sh -n"${name}" -u${ue}

.PHONY: env
env: .env ## Outputs the generated environment to console
	@./bin/env.sh -s

.PHONY: env-inject
env-inject: .env ## Outputs the generated environment to console and injects it into GHA env
	@./bin/env.sh -s -i

.PHONY: build package
package: build ## Forwards to build target
build: .env clean ## Builds the plugin
	@(. ./bin/env.sh; ./bin/build.sh)

.PHONY: test
test: .env ## Runs tests
	@if [[ "${nobuild}" == "true" ]]; then (. ./bin/env.sh; ./bin/test.sh); else (. ./bin/env.sh; ./bin/clean.sh; ./bin/build.sh; ./bin/test.sh) fi

.PHONY: clean
clean: ## Cleans up build artifacts
	@./bin/clean.sh

.PHONY: clean-all
clean-all: .env ## Cleans up build artifacts, as well as mock artifactory and symlinking for local dev
	@(. ./bin/env.sh; ./bin/clean.sh -a)

.PHONY: deploy
deploy: .env ## Deploys the artifact to artifactory
	@(. ./bin/env.sh; ./bin/deploy.sh)

.PHONY: docs
docs: ## Generates documentation
	@doxygen Doxyfile

.PHONY: dev
dev: ## Sets up a development environment
	@echo "Setting up development environment..."
	@echo ${env} > .env
	@echo ${ue} >> .env
	@echo $(shell [[ ! -z ${version} ]] && echo ${version} || head -1 VERSION | xargs) >> .env
	@echo ${target_platform} >> .env
