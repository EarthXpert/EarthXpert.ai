project(expy)

install(DIRECTORY api/ 
        DESTINATION py/api
        FILES_MATCHING PATTERN "*.py")
install(DIRECTORY app/cli/ 
        DESTINATION py/app/cli
        FILES_MATCHING PATTERN "*.py")
install(DIRECTORY app/gui/ 
        DESTINATION py/app/gui
        FILES_MATCHING PATTERN "*.py")
install(DIRECTORY tests/ 
        DESTINATION py/tests
        FILES_MATCHING PATTERN "*.py")
install(DIRECTORY utils/ 
        DESTINATION py/utils
        FILES_MATCHING PATTERN "*.py")
