project(expy)

install(DIRECTORY api/ 
        DESTINATION py/api
        FILES_MATCHING PATTERN "*.py")
install(DIRECTORY app/gui/ 
        DESTINATION py/app
        FILES_MATCHING PATTERN "*.py")
install(DIRECTORY utils/ 
        DESTINATION py/utils
        FILES_MATCHING PATTERN "*.py")
