# Copyright (C) 2026 Bryce Thuilot <bryce@thuilot.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the FSF, either version 3 of the License, or (at your option) any later version.
# See the LICENSE file in the root of this repository for full license text or
# visit: <https://www.gnu.org/licenses/gpl-3.0.html>.

CC ?= cc
CFLAGS := -std=c17 -Wall -Wextra -g -fsanitize=address -Isrc
LDFLAGS := -fsanitize=address

DAYS := $(patsubst src/days/day%.c,day%,$(wildcard src/days/day*.c))

CSRCS := $(wildcard src/days/day*.c) src/main.c src/aoc.h

BUILDDIR := $(shell pwd)/build
LOCALBIN := $(shell pwd)/bin

.PHONY: all clean tests run-tests
.SECONDARY:

##@ building

all: $(DAYS)

$(DAYS): day%: $(LOCALBIN)/day%
	@true

$(LOCALBIN)/day%: $(BUILDDIR)/main-%.o $(BUILDDIR)/day%.o | $(LOCALBIN)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/main-%.o: src/main.c src/aoc.h | $(BUILDDIR)
	$(CC) $(CFLAGS) -DDAY=$* -c $< -o $@

$(BUILDDIR)/day%.o: src/days/day%.c src/aoc.h | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(LOCALBIN):
	mkdir -p $(LOCALBIN)

clean:
	rm -rf build bin/day[0-9]*

tests: $(DAYS) $(LOCALBIN)/tests

$(LOCALBIN)/tests: tests/main.c | $(LOCALBIN)
	$(CC) $(CFLAGS) $< -o $@


##@ tests

run-tests: $(LOCALBIN)/tests
	PATH="$$PATH:$(LOCALBIN)" "$(LOCALBIN)/tests" $(if $(DAY),-d '$(DAY)') 

## dev tools
ifeq (,$(shell go env GOBIN))
GOBIN=$(shell go env GOPATH)/bin
else
GOBIN=$(shell go env GOBIN)
endif


##@ linting

.PHONY: lint
lint: license-eye ## run linters (license-eye and clang-format)
	clang-format --dry-run $(CSRCS) || echo "clang-format not installed, skipping"
	"$(LICENSE_EYE)" header check

.PHONY: lint-fix
lint-fix: license-eye ## run linters and perform fixes (license-eye and clang-format)
	clang-format -i $(CSRCS) || echo "clang-format not installed, skipping"
	"$(LICENSE_EYE)" header fix

.PHONY: license-fix
license-fix: ## run just license fixer
	@"$(LICENSE_EYE)" header fix

LICENSE_EYE ?= $(LOCALBIN)/license-eye

# tool versions
LICENSE_EYE_VERSION ?= v0.9.0

license-eye: $(LICENSE_EYE) ## Download skywalking-eyes locally if necessary.
$(LICENSE_EYE): $(LOCALBIN)
	$(call go-install-tool,$(LICENSE_EYE),github.com/apache/skywalking-eyes/cmd/license-eye,$(LICENSE_EYE_VERSION))

# go-install-tool will 'go install' any package with custom target and name of binary, if it doesn't exist
# $1 - target path with name of binary
# $2 - package url which can be installed
# $3 - specific version of package
define go-install-tool
@[ -f "$(1)-$(3)" ] && [ "$$(readlink -- "$(1)" 2>/dev/null)" = "$(1)-$(3)" ] || { \
set -e; \
package=$(2)@$(3) ;\
echo "Downloading $${package}" ;\
rm -f "$(1)" ;\
GOBIN="$(LOCALBIN)" go install $${package} ;\
mv "$(LOCALBIN)/$$(basename "$(1)")" "$(1)-$(3)" ;\
} ;\
ln -sf "$$(realpath "$(1)-$(3)")" "$(1)"
endef

define gomodver
$(shell go list -m -f '{{if .Replace}}{{.Replace.Version}}{{else}}{{.Version}}{{end}}' $(1) 2>/dev/null)
endef

