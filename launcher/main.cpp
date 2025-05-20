#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    pid_t pid = fork();

    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
        execl("./PassManager", "PassManager", nullptr);
        _exit(1);
    } else if (pid > 0) {

        int status;
        waitpid(pid, &status, 0);

        if (WIFSTOPPED(status)) {
            qDebug() << "Attached to child process with pid" << pid;

            ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            waitpid(pid, &status, 0);
            qDebug() << "Child exited with status" << status;
        } else {
            qDebug() << "Failed to attach to child process.";
        }
    } else {
        qDebug() << "Fork failed.";
    }

    return 0;
}
