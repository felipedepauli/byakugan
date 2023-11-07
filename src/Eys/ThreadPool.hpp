#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>

class ThreadPool {
public:
    // Constructor: Inicializa o thread pool com o número especificado de threads.
    ThreadPool(size_t threads);

    // Destructor: Sinaliza para as threads pararem e aguarda que todas terminem.
    ~ThreadPool();

    // Método para enfileirar uma tarefa no thread pool.
    // Retorna um std::future que pode ser usado para esperar que a tarefa seja concluída e obter seu resultado.
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;

        // Cria uma tarefa embalada que encapsula a função e seus argumentos.
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        // Obtém um std::future associado à tarefa.
        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // Não permite enfileirar tarefas depois de parar o pool.
            if(stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            // Enfileira a tarefa.
            tasks.emplace([task](){
                std::cout << "[ThreadPool::Info] Executing task in thread " << std::this_thread::get_id() << std::endl;
                (*task)();
            });
        }

        // Notifica uma thread esperando para pegar uma tarefa.
        condition.notify_one();
        return res;
    }

private:
    // Lista de threads de trabalho.
    std::vector<std::thread> workers;

    // Fila de tarefas.
    std::queue<std::function<void()>> tasks;

    // Mutex para sincronização.
    std::mutex queue_mutex;

    // Variável de condição para sinalizar threads.
    std::condition_variable condition;

    // Flag para indicar se o pool de threads está parando.
    bool stop;
};
