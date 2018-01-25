defmodule TimeServer.Supervisor do
  use Supervisor
  require Logger

  @name TimeServer.Supervisor

  def start_link(port) do
    Supervisor.start_link(__MODULE__, [:ok, port], name: @name)
  end

  def init([:ok, port]) do
    children = [
      supervisor(Task.Supervisor, [[name: TimeServer.TaskSupervisor]]),
      worker(Task, [TimeServer.Server, :accept, [port]])
    ]

    opts = [strategy: :one_for_one, name: TimeServer.Supervisor]
    supervise(children, opts)
  end
end
