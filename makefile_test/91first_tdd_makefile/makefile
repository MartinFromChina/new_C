TARGET = test
CC = g++

# We define the global AM_CPPFLAGS as everything we compile includes from these
# directories.
AM_CPPFLAGS = -I$(srcdir) -I$(srcdir)/include

# gtest source files that we don't compile directly.  They are
# #included by gtest-all.cc.
GTEST_SRC = \
  ../../tdd/google/gmock-1.7.0/gtest/src/gtest-death-test.cc \
  ../../tdd/google/gmock-1.7.0/gtest/src/gtest-filepath.cc \
  ../../tdd/google/gmock-1.7.0/gtest/src/gtest-internal-inl.h \
  ../../tdd/google/gmock-1.7.0/gtest/src/gtest-port.cc \
  ../../tdd/google/gmock-1.7.0/gtest/src/gtest-printers.cc \
  ../../tdd/google/gmock-1.7.0/gtest/src/gtest-test-part.cc \
  ../../tdd/google/gmock-1.7.0/gtest/src/gtest-typed-test.cc \
  ../../tdd/google/gmock-1.7.0/gtest/src/gtest.cc
  
gtest_pkginclude_HEADERS = \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest-death-test.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest-message.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest-param-test.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest-printers.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest-spi.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest-test-part.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest-typed-test.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest_pred_impl.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/gtest_prod.h

gtest_pkginclude_internal_HEADERS = \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-death-test-internal.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-filepath.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-internal.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-linked_ptr.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-param-util-generated.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-param-util.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-port.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-string.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-tuple.h \
  ../../tdd/google/gmock-1.7.0/gtest/include/gtest/internal/gtest-type-util.h
  
gmock_GMOCK_SOURCE_INGLUDES = \
  ../../tdd/google/gmock-1.7.0/src/gmock-cardinalities.cc \
  ../../tdd/google/gmock-1.7.0/src/gmock-internal-utils.cc \
  ../../tdd/google/gmock-1.7.0/src/gmock-matchers.cc \
  ../../tdd/google/gmock-1.7.0/src/gmock-spec-builders.cc \
  ../../tdd/google/gmock-1.7.0/src/gmock.cc

gmock_pkginclude_HEADERS = \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-actions.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-cardinalities.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-generated-actions.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-generated-function-mockers.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-generated-matchers.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-generated-nice-strict.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-matchers.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-more-actions.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-more-matchers.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock-spec-builders.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/gmock.h

gmock_pkginclude_internal_HEADERS = \
  ../../tdd/google/gmock-1.7.0/include/gmock/internal/gmock-generated-internal-utils.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/internal/gmock-internal-utils.h \
  ../../tdd/google/gmock-1.7.0/include/gmock/internal/gmock-port.h
 

SRCS += $(gtest_pkginclude_HEADERS)
SRCS += $(gtest_pkginclude_internal_HEADERS)
SRCS += $(gmock_pkginclude_HEADERS)
SRCS += $(gmock_pkginclude_internal_HEADERS)
SRCS += $(GTEST_SRC)
SRCS += $(gmock_GMOCK_SOURCE_INGLUDES)
  
$(TARGET):$(SRCS)
	$(CC) $(SRCS) -o $(TARGET)

clean :
	rm -rf $(TARGET)








