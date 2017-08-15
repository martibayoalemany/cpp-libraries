from conan.packager import ConanMultiPackager


if __name__ == "__main__":
    builder = ConanMultiPackager(username="user", channel="stable")
    builder.add_common_builds()
    builder.run()
